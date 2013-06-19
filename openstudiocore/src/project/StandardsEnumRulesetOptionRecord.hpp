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

#ifndef PROJECT_STANDARDSENUMRULESETOPTIONRECORD_HPP
#define PROJECT_STANDARDSENUMRULESETOPTIONRECORD_HPP

#include "ProjectAPI.hpp"
#include <project/StandardsRulesetOptionRecord.hpp>

#include <utilities/core/Logger.hpp>
#include <utilities/idd/IddFactory.hxx>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class StandardsEnumRulesetOption;
}
namespace project {

namespace detail {
  class StandardsEnumRulesetOptionRecord_Impl;
}

class StandardsRulesetRecord;

/** StandardsEnumRulesetOptionRecord 
**/
class PROJECT_API StandardsEnumRulesetOptionRecord : public StandardsRulesetOptionRecord {
 public:

  typedef detail::StandardsEnumRulesetOptionRecord_Impl ImplType;
  typedef RulesetOptionRecordColumns ColumnsType;
  typedef RulesetOptionRecord ObjectRecordType;

  /// constructors
  StandardsEnumRulesetOptionRecord(
    const openstudio::ruleset::StandardsEnumRulesetOption& standardsEnumRulesetOption, 
    const StandardsRulesetRecord& standardsRulesetRecord);

  /// constructors
  StandardsEnumRulesetOptionRecord(
    const openstudio::ruleset::StandardsEnumRulesetOption& standardsEnumRulesetOption, 
    ProjectDatabase& projectDatabase);

  /// constructor from query, throws if bad query
  StandardsEnumRulesetOptionRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase);

  virtual ~StandardsEnumRulesetOptionRecord() {} 

  static boost::optional<StandardsEnumRulesetOptionRecord> factoryFromQuery(const QSqlQuery& query, 
                                                                            ProjectDatabase& database);

  /// get all model records in database
  static std::vector<StandardsEnumRulesetOptionRecord> getStandardsEnumRulesetOptionRecords(
      ProjectDatabase& projectDatabase);

  /// get model record in database
  static boost::optional<StandardsEnumRulesetOptionRecord> getStandardsEnumRulesetOptionRecord(
      int id, ProjectDatabase& projectDatabase);

  /// get the ruleset option from the record
  openstudio::ruleset::StandardsEnumRulesetOption standardsEnumRulesetOption() const;

 protected:

  friend class Record;
  friend class ProjectDatabase;

  // from impl
  StandardsEnumRulesetOptionRecord(
      boost::shared_ptr<detail::StandardsEnumRulesetOptionRecord_Impl> impl, 
      ProjectDatabase projectDatabase);

  // from impl, does not register in database, use with caution
  StandardsEnumRulesetOptionRecord(boost::shared_ptr<detail::StandardsEnumRulesetOptionRecord_Impl> impl);
    
 private:
  REGISTER_LOGGER("openstudio.project.StandardsEnumRulesetOptionRecord");
};

/** \relates StandardsEnumRulesetOptionRecord */
typedef boost::optional<StandardsEnumRulesetOptionRecord> OptionalStandardsEnumRulesetOptionRecord;

/** \relates StandardsEnumRulesetOptionRecord */
typedef std::vector<StandardsEnumRulesetOptionRecord> StandardsEnumRulesetOptionRecordVector;

} // project
} // openstudio

#endif // PROJECT_STANDARDSENUMRULESETOPTIONRECORD_HPP
