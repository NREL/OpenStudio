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

#ifndef PROJECT_MODELRULERECORD_HPP
#define PROJECT_MODELRULERECORD_HPP

#include "ProjectAPI.hpp"
#include <project/RuleRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset{
  class ModelRule;
}

namespace project {

  namespace detail {
    class ModelRuleRecord_Impl;
  }


  /** ModelRuleRecord 
  **/
  class PROJECT_API ModelRuleRecord : public RuleRecord {
    public:

      typedef detail::ModelRuleRecord_Impl ImplType;
      typedef RuleRecordColumns ColumnsType;
      typedef RuleRecord ObjectRecordType;

      /// constructors
      ModelRuleRecord(const openstudio::ruleset::ModelRule& modelRule, ProjectDatabase& projectDatabase);

      /// constructor from query, throws if bad query
      ModelRuleRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase);

      virtual ~ModelRuleRecord() {}

      static boost::optional<ModelRuleRecord> factoryFromQuery(const QSqlQuery& query, 
                                                               ProjectDatabase& database);

      /// get all model records in database
      static std::vector<ModelRuleRecord> getModelRuleRecords(ProjectDatabase& projectDatabase);

      /// get model record in database
      static boost::optional<ModelRuleRecord> getModelRuleRecord(int id, ProjectDatabase& projectDatabase);

      /// get the model rule
      openstudio::ruleset::ModelRule modelRule() const;

    protected:

      friend class Record;
      friend class ProjectDatabase;

      // from impl
      ModelRuleRecord(boost::shared_ptr<detail::ModelRuleRecord_Impl> impl, ProjectDatabase projectDatabase);

      // from impl, does not register in database, use with caution
      ModelRuleRecord(boost::shared_ptr<detail::ModelRuleRecord_Impl> impl);

    private:

      REGISTER_LOGGER("openstudio.project.ModelRuleRecord");
  };

  /** \relates ModelRuleRecord */
  typedef boost::optional<ModelRuleRecord> OptionalModelRuleRecord;

  /** \relates ModelRuleRecord */
  typedef std::vector<ModelRuleRecord> ModelRuleRecordVector;

} // project
} // openstudio

#endif // PROJECT_MODELRULERECORD_HPP
