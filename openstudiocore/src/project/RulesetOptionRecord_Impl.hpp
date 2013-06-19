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

#ifndef PROJECT_RULESETOPTIONRECORD_IMPL_HPP
#define PROJECT_RULESETOPTIONRECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include <project/ObjectRecord_Impl.hpp>
#include <project/RulesetOptionRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class RulesetOption;
}
namespace project {

  class RulesetRecord;
  class RulesetOptionRecord;

namespace detail {

  class PROJECT_API RulesetOptionRecord_Impl : public ObjectRecord_Impl
  {

    Q_OBJECT;

    public:

      /// constructors
      RulesetOptionRecord_Impl(const openstudio::ruleset::RulesetOption& rulesetOption, const RulesetOptionRecordType& rulesetOptionRecordType, const RulesetRecord& rulesetRecord);

      /// constructors
      RulesetOptionRecord_Impl(const openstudio::ruleset::RulesetOption& rulesetOption, const RulesetOptionRecordType& rulesetOptionRecordType, const ProjectDatabase& projectDatabase);

      /// constructor from query, throws if bad query
      RulesetOptionRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase);

      virtual ~RulesetOptionRecord_Impl() {}

      /// get name of the database table
      virtual std::string databaseTableName() const;

      /// get the related ruleset record
      boost::optional<RulesetRecord> rulesetRecord() const;

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

      REGISTER_LOGGER("openstudio.project.RulesetOptionRecord");

      RulesetOptionRecordType m_rulesetOptionRecordType;
      boost::optional<int> m_rulesetRecordId;

      RulesetOptionRecordType m_lastRulesetOptionRecordType;
      boost::optional<int> m_lastRulesetRecordId;

  };

} // detail
} // project
} // openstudio

#endif // PROJECT_RULESETOPTIONRECORD_IMPL_HPP
