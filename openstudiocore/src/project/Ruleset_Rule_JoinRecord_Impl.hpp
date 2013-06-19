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

#ifndef PROJECT_RULESET_RULE_JOINRECORD_IMPL_HPP
#define PROJECT_RULESET_RULE_JOINRECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include <project/JoinRecord_Impl.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace project {

  class RulesetRecord;
  class RuleRecord;

namespace detail {

  class PROJECT_API Ruleset_Rule_JoinRecord_Impl : public JoinRecord_Impl
  {

    Q_OBJECT;

    public:

      /// constructors
      Ruleset_Rule_JoinRecord_Impl(const RulesetRecord& rulesetRecord, const RuleRecord& ruleRecord);

      /// constructor from query, throws if bad query
      Ruleset_Rule_JoinRecord_Impl(const QSqlQuery& query, const ProjectDatabase& database);

      virtual ~Ruleset_Rule_JoinRecord_Impl() {}
            
      /// get the left object
      virtual ObjectRecord leftObject() const;

      /// get the right object
      virtual ObjectRecord rightObject() const;

      /// get ruleset record
      RulesetRecord rulesetRecord() const;

      /// get rule record
      RuleRecord ruleRecord() const;

      /// get name of the database table, implement at table level
      virtual std::string databaseTableName() const;

      /// save the row for just this object in the database
      /// requires ProjectDatabase as called in ProjectDatabase ctor/dtor
      virtual void saveRow(const boost::shared_ptr<QSqlDatabase> &database);

    private:

      REGISTER_LOGGER("openstudio.project.Ruleset_Rule_JoinRecord");

  };

} // detail
} // project
} // openstudio

#endif // PROJECT_RULESET_RULE_JOINRECORD_IMPL_HPP
